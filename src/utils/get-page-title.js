import defaultSettings from '@/settings'

const title = defaultSettings.title || 'Government affairs system'

export default function getPageTitle(pageTitle) {
  if (pageTitle) {
    return `${pageTitle} - ${title}`
  }
  return `${title}`
}
